#ifndef __CERBERUS_COMMAND_HPP__
#define __CERBERUS_COMMAND_HPP__

#include <vector>

#include "utils/pointer.h"
#include "typetraits.hpp"
#include "buffer.hpp"

struct iovec;

namespace cerb {

    class Client;
    class CommandGroup;

    class Command {
    public:
        Buffer buffer;
        util::sref<CommandGroup> group;
        bool need_send;

        virtual ~Command() {}

        void copy_response(Buffer::iterator begin, Buffer::iterator end);

        Command(Buffer b, util::sref<CommandGroup> g, bool n)
            : buffer(std::move(b))
            , group(g)
            , need_send(n)
        {}

        Command(Command const&) = delete;
    };

    class CommandGroup {
    public:
        util::sref<Client> client;
        Buffer arr_payload;
        std::vector<util::sptr<Command>> commands;
        int awaiting_count;

        CommandGroup(CommandGroup const&) = delete;

        explicit CommandGroup(util::sref<Client> c)
            : client(c)
            , awaiting_count(0)
        {}

        void command_responsed();
        void append_command(util::sptr<Command> c);
        void append_buffer_to(std::vector<struct iovec>& iov);
        int total_buffer_size() const;
    };

    std::vector<util::sptr<CommandGroup>> split_client_command(
        Buffer& buffer, util::sref<Client> cli);

}

#endif /* __CERBERUS_COMMAND_HPP__ */
