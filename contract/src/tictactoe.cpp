#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>

using namespace eosio;

// The contract
CONTRACT tictactoe : public contract {
  public:
  
  using contract::contract;

    tictactoe(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
      hodl_symbol("SYS", 4) {} //singleton initialize    

    TABLE game_record {
      name host;
      name opponent;
      eosio::asset hoststake;
      eosio::asset opponentstake;

      uint64_t primary_key() const {return host.value;}
      uint128_t secondary_key() const {return combine_ids(host.value,opponent.value);}
    };
	
    typedef eosio::multi_index<name("games"), game_record,
      eosio::indexed_by<name("gameskey"), eosio::const_mem_fun<game_record, uint128_t, &game_record::secondary_key>>
    > game_index;

    ACTION welcome(name host, name opponent) {
        //action can only be taken by contract account
        require_auth(get_self());
        print("Welcome, ", host, " and ", opponent,"!");
		print(hodl_symbol);
    }

	[[eosio::on_notify("eosio.token::transfer")]]
    void create(name from, name to, eosio::asset quantity, std::string memo) {
      if (from == get_self() || to != get_self())
      {
        return;
      }

      check(quantity.amount > 0, "When pigs fly");
      check(quantity.symbol == hodl_symbol, "These are not the droids you are looking for.");
 
	  name opponent = name(memo);

    print (from);
    print (opponent);
    print (quantity);
	  
	  check(is_account(opponent),"opponent account not found");
      check(from!=opponent, "Could not challenge youself!");
        
      game_index _game(get_self(), get_self().value);
      //find using secondary key
      auto _gameskey = _game.get_index<name("gameskey")>(); //set secondary key
      auto itrh = _gameskey.find(combine_ids(from.value, opponent.value));
      auto itrc = _gameskey.find(combine_ids(opponent.value, from.value));
      if (itrh==_gameskey.end() && itrc==_gameskey.end()) {
		//ram charge to action caller
        _game.emplace(get_self(), [&](auto& pair) { 
          pair.host = from;
          pair.opponent = opponent;
    	  pair.hoststake = quantity;
	      pair.opponentstake = {0.0000,hodl_symbol};
        print(pair.hoststake);
        print(pair.opponentstake);        
        });
	  }
	  else if (itrh!=_gameskey.end()) {
		//ram charge to same_payer
        print("itrh");
        print(itrh->hoststake);
        _game.modify(*itrh, same_payer, [&](auto& pair) { 
		    //pair.hoststake += quantity;
        });  
	  }
	  else {
		//ram charge to same_payer
        print("itrc");
        print(itrc->opponentstake);
        _game.modify(*itrc, same_payer, [&](auto& pair) { 
		    //pair.opponentstake += quantity;
        });  		  
	  }
    }

    ACTION close(name opponent, name host) {
        //find by host (primary key)
        check(has_auth(host) || has_auth(opponent), "Please auth yourself.");
        game_index _game(get_self(), get_self().value);
        //find using secondary key
        auto _gameskey = _game.get_index<name("gameskey")>();
        auto itrc = _gameskey.find(combine_ids(host.value, opponent.value));          
        if (itrc!=_gameskey.end())
          _gameskey.erase(itrc);
        else {
          auto itrp = _gameskey.find(combine_ids(opponent.value, host.value));          
          if (itrp!=_gameskey.end())
            _gameskey.erase(itrp);
          else 
            check(false, "Game not found.");
        }
    }   

  private: 
    const symbol hodl_symbol;
  
    // concatenation of ids example
    static uint128_t combine_ids(const uint64_t &x, const uint64_t &y) {
      return (uint128_t{x} << 64) | y;
    }

};