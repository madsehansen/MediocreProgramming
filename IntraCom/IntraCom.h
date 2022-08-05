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

    template <typename T>
    struct CommandData;

    class DataReader
    {
    public:
        virtual ~DataReader() = default;

        auto generation() const->Generation
        {
            return m_generation;
        }

        void makeCallback()
        {
            m_callback( this );
        }

    protected:
        explicit DataReader(
            std::function<void( DataReader* )> a_callback )
            : m_callback { std::move( a_callback ) }
        {
        }

        std::function< void( DataReader* )> m_callback;
        Generation m_generation { 0 };
    };

    template <typename T>
    class CommandReader : public DataReader
    {
    public:
        explicit CommandReader(
            std::function<void( DataReader* )> a_callback,
            CommandData<T>* a_commandData )
            : DataReader( std::move( a_callback ) )
            , m_commandData { a_commandData }
        {
            m_commandData->m_readers.push_back( this );
            m_generation = m_commandData->m_generation;
        }

        ~CommandReader() override = default;

        auto read()->std::vector<T>;

    private:
        CommandData<T>* m_commandData { nullptr };
    };

    class DataWriter
    {
    public:
        virtual ~DataWriter() = default;

    protected:
        explicit DataWriter( )
        {
        }
    };

    template <typename T>
    class CommandWriter : public DataWriter
    {
    public:
        explicit CommandWriter(
            CommandData<T>* a_commandData )
            : m_commandData { a_commandData }
        {
            m_commandData->m_writers.push_back( this );
        }

        ~CommandWriter() override = default;

        void write( const T& a_data );

    private:
        CommandData<T>* m_commandData { nullptr };
    };

    template <typename T>
    struct CommandData
    {
        std::recursive_mutex m_mutexData;
        std::recursive_mutex m_mutexStructure;
        std::vector<DataReader*> m_readers;
        std::vector<DataWriter*> m_writers;
        std::vector<DataElement<T>> m_data;
        Generation m_generation { 0 };
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
        auto getCommandReader( std::function<void( DataReader* )> a_callback )->CommandReader< T >*;

        template <typename T>
        auto getCommandWriter()->CommandWriter< T >*;

        void start();
        void stop();

    private:

        struct ICommandData
        {
            virtual ~ICommandData() = default;
            virtual auto getTypeIndex() const->std::type_index = 0;
            virtual auto createDataReader( std::function<void( DataReader* )> a_callback  )->DataReader* = 0;
            virtual auto createDataWriter()->DataWriter* = 0;
            virtual void runReaders() = 0;
        };

        template <typename T>
        struct ConcreteData : public ICommandData
        {
            CommandData< T > data;


            // Inherited via ICommandData
            auto getTypeIndex() const -> std::type_index override
            {
                return std::type_index( typeid( T ) );
            }

            auto createDataReader( std::function<void( DataReader* )> a_callback ) -> DataReader* override
            {
                std::lock_guard lockStructure( data.m_mutexStructure );

                return new CommandReader<T>( std::move( a_callback ), &data );
            }

            auto createDataWriter() -> DataWriter* override
            {
                std::lock_guard lockStructure( data.m_mutexStructure );

                return new CommandWriter<T>( &data );
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

        std::vector<std::unique_ptr<ICommandData>> m_commandData;

        bool m_keepRunning { false };
        std::thread m_thread;
    };

    template<typename T>
    inline void CommandWriter<T>::write( const T& a_data )
    {
        std::lock_guard lockData( m_commandData->m_mutexData );
        
        ++m_commandData->m_generation;

        m_commandData->m_data.emplace_back( m_commandData->m_generation, a_data );
    }

    template<typename T>
    inline auto CommandReader<T>::read() -> std::vector<T>
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
    inline auto IntraCom::getCommandReader( std::function<void( DataReader* )> a_callback ) -> CommandReader<T>*
    {
        auto existing = std::find_if(
            m_commandData.begin(),
            m_commandData.end(),
            [ type = std::type_index( typeid( T ) ) ]( const auto& commandData ) { return type == commandData->getTypeIndex(); } );

        if ( existing != m_commandData.end() )
            return static_cast< CommandReader<T>* >( (*existing)->createDataReader( a_callback ) );
        else
        {
            auto& newData { m_commandData.emplace_back( new ConcreteData< T >() ) };
            return static_cast<CommandReader<T>*>( newData->createDataReader( a_callback ) );
        }
    }

    template<typename T>
    inline auto IntraCom::getCommandWriter() -> CommandWriter<T>*
    {
        auto existing = std::find_if(
            m_commandData.begin(),
            m_commandData.end(),
            [ type = std::type_index( typeid( T ) ) ]( const auto& commandData ) { return type == commandData->getTypeIndex(); } );

        if ( existing != m_commandData.end() )
            return static_cast<CommandWriter<T>*>( (*existing)->createDataWriter( ) );
        else
        {
            auto& newData { m_commandData.emplace_back( new ConcreteData< T >() ) };
            return static_cast<CommandWriter<T>*>( newData->createDataWriter( ) );
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
