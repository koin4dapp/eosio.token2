#include <eosio/eosio.hpp>

using namespace eosio;

// The contract
CONTRACT tictactoe : public contract {
  public:
    // Use contract's constructor
    using contract::contract;

    ACTION create (name host) {
        //action can only be taken by contract account
        require_auth(get_self());
        print("Hello, ", host, ".");
    }
};
