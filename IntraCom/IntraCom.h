#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <mutex>
#include <thread>
#include <typeindex>
#include <typeinfo>
#include <vector>

namespace IntraCom
{
    using Generation = uint64_t;

    template <typename T>
    struct DataElement
    {
        Generation generation;
        T data;
    };

    class Reader
    {
    public:
        virtual ~Reader() = default;

        auto generation() const->Generation
        {
            return m_generation;
        }

        void makeCallback()
        {
            m_callback( this );
        }

    protected:
        explicit Reader(
            std::function<void( Reader* )> a_callback )
            : m_callback { std::move( a_callback ) }
        {
        }

        std::function< void( Reader* )> m_callback;
        Generation m_generation { 0 };
    };

    class Writer
    {
    public:
        virtual ~Writer() = default;

    protected:
        explicit Writer()
        {
        }
    };


    template <typename T>
    struct IntraComData
    {
        std::recursive_mutex m_mutexData;
        std::recursive_mutex m_mutexStructure;
        std::vector<std::unique_ptr<Reader>> m_readers;
        std::vector<std::unique_ptr<Writer>> m_writers;
        std::vector<DataElement<T>> m_data;
        Generation m_generation { 0 };
    };


    template <typename T>
    class DataReader : public Reader
    {
    public:
        explicit DataReader(
            std::function<void( Reader* )> a_callback,
            IntraComData<T>* a_commandData )
            : Reader( std::move( a_callback ) )
            , m_commandData { a_commandData }
        {
            m_commandData->m_readers.emplace_back( this );
            m_generation = 0;
        }

        ~DataReader() override = default;

        auto read()->std::vector<T>;

    private:
        IntraComData<T>* m_commandData { nullptr };
    };

    template <typename T>
    class DataWriter : public Writer
    {
    public:
        explicit DataWriter(
            IntraComData<T>* a_commandData )
            : m_commandData { a_commandData }
        {
            m_commandData->m_writers.emplace_back( this );
        }

        ~DataWriter() override = default;

        void write( const T& a_data );

    private:
        IntraComData<T>* m_commandData { nullptr };
    };



    class IntraCom
    {
    public:
        IntraCom() = default;
        ~IntraCom()
        {
            if ( m_keepRunning )
                stop();
        }

        template <typename T>
        auto createReader( std::function<void( Reader* )> a_callback )->DataReader< T >*;

        template <typename T>
        auto createWriter()->DataWriter< T >*;

        void start();
        void stop();

    private:

        struct ICommonData
        {
            virtual ~ICommonData() = default;
            virtual auto getTypeIndex() const->std::type_index = 0;
            virtual auto createDataReader( std::function<void( Reader* )> a_callback  )->Reader* = 0;
            virtual auto createDataWriter()->Writer* = 0;
            virtual void runReaders() = 0;
        };

        template <typename T>
        struct CommonData : public ICommonData
        {
            IntraComData< T > data;


            // Inherited via ICommandData
            auto getTypeIndex() const -> std::type_index override
            {
                return std::type_index( typeid( T ) );
            }

            auto createDataReader( std::function<void( Reader* )> a_callback ) -> Reader* override
            {
                std::lock_guard lockStructure( data.m_mutexStructure );

                return new DataReader<T>( std::move( a_callback ), &data );
            }

            auto createDataWriter() -> Writer* override
            {
                std::lock_guard lockStructure( data.m_mutexStructure );

                return new DataWriter<T>( &data );
            }

            void runReaders() override
            {
                std::lock_guard lockStructure( data.m_mutexStructure );

                for ( auto& reader : data.m_readers )
                    if ( data.m_generation > reader->generation() )
                        reader->makeCallback();

                // Find last element read by all readers
                std::lock_guard lockData( data.m_mutexData );

                Generation minGeneration { data.m_generation };
                for ( auto& reader : data.m_readers )
                    minGeneration = std::min( reader->generation(), minGeneration );

                // Remove all elements read by all readers, to preserve space
                data.m_data.erase(
                    data.m_data.begin(),
                    std::find_if(
                        data.m_data.begin(),
                        data.m_data.end(),
                        [=]( const DataElement< T >& element ) { return element.generation > minGeneration; } )
                );
            }
        };

        void runReaders();

        std::vector<std::unique_ptr<ICommonData>> m_commandData;

        bool m_keepRunning { false };
        std::thread m_thread;
    };

    template<typename T>
    inline void DataWriter<T>::write( const T& a_data )
    {
        std::lock_guard lockData( m_commandData->m_mutexData );
        
        ++m_commandData->m_generation;

        m_commandData->m_data.emplace_back( m_commandData->m_generation, a_data );
    }

    template<typename T>
    inline auto DataReader<T>::read() -> std::vector<T>
    {
        std::lock_guard lockData( m_commandData->m_mutexData );
        
        auto currGen { m_generation };
        m_generation = m_commandData->m_generation;

        auto first { std::find_if(
                m_commandData->m_data.begin(),
                m_commandData->m_data.end(),
                [=]( const DataElement<T>& data ) { return data.generation > currGen; } ) };

        std::vector<T> result;
        result.reserve( std::distance( first, m_commandData->m_data.end() ) );
        std::transform(
            first,
            m_commandData->m_data.end(),
            std::back_inserter( result ),
            std::mem_fn( &DataElement< T >::data ) );

        return result;
    }

    template<typename T>
    inline auto IntraCom::createReader( std::function<void( Reader* )> a_callback ) -> DataReader<T>*
    {
        auto existing = std::find_if(
            m_commandData.begin(),
            m_commandData.end(),
            [ type = std::type_index( typeid( T ) ) ]( const auto& commandData ) { return type == commandData->getTypeIndex(); } );

        if ( existing != m_commandData.end() )
            return static_cast< DataReader<T>* >( (*existing)->createDataReader( a_callback ) );
        else
        {
            auto& newData { m_commandData.emplace_back( new CommonData< T >() ) };
            return static_cast<DataReader<T>*>( newData->createDataReader( a_callback ) );
        }
    }

    template<typename T>
    inline auto IntraCom::createWriter() -> DataWriter<T>*
    {
        auto existing = std::find_if(
            m_commandData.begin(),
            m_commandData.end(),
            [ type = std::type_index( typeid( T ) ) ]( const auto& commandData ) { return type == commandData->getTypeIndex(); } );

        if ( existing != m_commandData.end() )
            return static_cast<DataWriter<T>*>( (*existing)->createDataWriter( ) );
        else
        {
            auto& newData { m_commandData.emplace_back( new CommonData< T >() ) };
            return static_cast<DataWriter<T>*>( newData->createDataWriter( ) );
        }
    }

    inline void IntraCom::start()
    {
        m_keepRunning = true;
        m_thread = std::thread( [this]() { this->runReaders(); } );
    }

    inline void IntraCom::stop()
    {
        m_keepRunning = false;
        m_thread.join();
    }

    inline void IntraCom::runReaders()
    {
        try
        {
            while ( m_keepRunning )
            {
                std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

                for ( auto& topic : m_commandData )
                    topic->runReaders();
            }
        }
        catch ( const std::exception& ex )
        {
            std::cerr << "IntraCom caught exception from callback: " << ex.what() << std::endl;
        }
    }

}
