## Building sample contract

The source code for the sample smartcontract is at `contract/talk.cpp` within the IDE. To compile the contract, run this in a terminal:

```
cd contract
eosio-cpp -o eosio.token.wasm src/eosio.token.cpp -I include --abigen

```

This will produce `eosio.token.abi` and `eosio.token.wasm`.

## Installing the contract

Run this in a terminal:

```
cleos create account eosio eosio.token EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

## this contract needs an eosio.code permission
cleos set account permission eosio.token active --add-code

cleos set code eosio.token eosio.token.wasm
cleos set abi eosio.token eosio.token.abi

```

## Creating users and using the contract

Run this in a terminal:
```
cleos create account eosio bob EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio jane EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

## Create SYS Token, and eosio.token as issuer
cleos push action eosio.token create '[ "eosio.token", "1000000000.0000 SYS"]' -p eosio.token@active

cleos get table eosio.token SYS stat

## Issue 100 SYS Token to eosio.token account
cleos push action eosio.token issue '[ "eosio.token", "100.0000 SYS", "memo" ]' -p eosio.token@active

cleos get table eosio.token SYS stat

## Transfer 25 SYS Token from eosio.token to bob
cleos push action eosio.token transfer '[ "eosio.token", "bob", "25.0000 SYS", "m" ]' -p eosio.token@active

## Transfer 15 SYS Token from bob to jane
cleos push action eosio.token transfer '[ "bob", "jane", "15.0000 SYS", "m" ]' -p bob@active

cleos get table eosio.token eosio.token accounts
cleos get table eosio.token bob accounts
cleos get table eosio.token jane accounts

cleos get table eosio.token bob stake

## Building and running the unit test

The source code for the unit test is at the `tests` directory within the IDE. To build the tests, run this in the terminal:

```
./build-tests

```

This will produce the `tester` binary, which can be run from the terminal to start the actual unit test:

```
./tester

```


## Resetting the chain

To remove the existing chain and create another:

* Switch to the terminal running `nodeos`
* Press `ctrl+c` to stop it
* Run the following

```
rm -rf ~/eosio/chain
nodeos --config-dir ~/eosio/chain/config --data-dir ~/eosio/chain/data -e -p eosio --plugin eosio::chain_api_plugin --contracts-console
