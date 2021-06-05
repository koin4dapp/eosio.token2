## Building sample contract

The source code for the sample smartcontract is at `contract/talk.cpp` within the IDE. To compile the contract, run this in a terminal:

```
eosio-cpp src/eosio.token.cpp -I include

```

This will produce `talk.abi` and `talk.wasm`.

## Installing the contract

Run this in a terminal:

```
cleos create account eosio talk EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos set code talk talk.wasm
cleos set abi talk talk.abi

```

## Creating users and using the contract

Run this in a terminal:
```
cleos create account eosio bob EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio jane EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos push action talk post '[1000, 0, bob, "This is a new post"]' -p bob
cleos push action talk post '[2000, 0, jane, "This is my first post"]' -p jane
cleos push action talk post '[1001, 2000, bob, "Replying to your post"]' -p bob

```

## Listing the messages

Run this in a terminal:
```
cleos get table talk '' message

```

## Viewing the front-end decentralized web app (EOSJS):

The source code for the React WebApp is at `webapp/src/index.tsx` within the IDE. To preview the WebApp run this in a terminal:

```
gp preview $(gp url 8000)

```

## Building and running the unit test

The source code for the unit test is at the `tests` directory within the IDE. To build the tests, run this in the terminal:

```
./build-tests

```

This will produce the `tester` binary, which can be run from the terminal to start the actual unit test:

```
./tester

```

The unit test creates the `talk_tests` test suite and verifies that the following statements are executed without error:

1. Create user account `talk`.
2. Load the `talk` smart contract in the `talk` account sandbox.
2. Create user accounts `john` and `jane`.
3. Test the `post` action by performing the following:
   1. Push the `post` action from `talk` to `john` with message "`post 1`" identified as `1` and addressed to message `0` (sent by noone).  
      This posts the message `1` from `john` to noone in the chat.
   2. Push the `post` action from `talk` to `jane` with message "`post 2`" identified as `2` and addressed to message `0` (sent by noone).  
      This posts the message `2` from `jane` to noone in the chat.
   3. Push the `post` action from `talk` to `john` with message "`post 3: reply`" identified as `3` and addressed to message `2` (sent by `jane`).  
      This posts the reply message `3` from `john` to `jane` in the chat.
4. Test failure of the `post` action if message is addressed to a non-existant message id.

## Resetting the chain

To remove the existing chain and create another:

* Switch to the terminal running `nodeos`
* Press `ctrl+c` to stop it
* Run the following

```
rm -rf ~/eosio/chain
nodeos --config-dir ~/eosio/chain/config --data-dir ~/eosio/chain/data -e -p eosio --plugin eosio::chain_api_plugin --contracts-console

```

Note: if the web app is currently open, then it will cause errors like the following. You may ignore them:

```
FC Exception encountered while processing chain.get_table_rows
```

## Contributing

[Contributing Guide](./CONTRIBUTING.md)

[Code of Conduct](./CONTRIBUTING.md#conduct)

## License

[MIT](./LICENSE)

## Important

See [LICENSE](LICENSE) for copyright and license terms.

All repositories and other materials are provided subject to the terms of this [IMPORTANT](important.md) notice and you must familiarize yourself with its terms.  The notice contains important information, limitations and restrictions relating to our software, publications, trademarks, third-party resources, and forward-looking statements.  By accessing any of our repositories and other materials, you accept and agree to the terms of the notice.
