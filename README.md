# BitcoinEco: proof of stake cryptocurrency driven by environmental entropy
-----------------

## What is BitcoinEco?

Bitcoin is an experimental digital currency that enables instant payments to
anyone, anywhere in the world. Bitcoin uses peer-to-peer technology to operate
with no central authority: managing transactions and issuing money are carried
out collectively by the network. Bitcoin Core is the name of open source
software which enables the use of this currency.

BitcoinEco forks Bitcoin, replacing proof of work mining with an environmentally driven proof of stake model.
Where other proof of stake systems use entropy generated within the network of peers to determine the peer who may claim and issue the next block, BitcoinEco participants use publicly verifiable observations of the environment and universe to obtain the entropy used for stake selection.
The security of this process is guaranteed as long as the entropy required for block choice is no greater than the amount of trustable input into the system.

BitcoinEco is ready to be used to initate a fork of Bitcoin classic.
To boot the fork, we need peers who have a full copy of the bitcoin blockchain. 
They will also need bitcoin in the existing chain, which can be staked. 
Staking participants will select the next block winner among each other using entropy from an earth-facing geostationary weather satellite.

This repo is a clean git fork of Bitcoin core.
Continued development there can be pulled in without issue provided it does not overlap with a small number of functions related to the proof of stake system.

------------

## License

BitcoinEco is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

## Building

On Ubuntu 18.04, first install dependencies:

```bash
sudo apt install build-essential libtool autotools-dev autoconf automake pkg-config libssl-dev libboost-all-dev libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler libqrencode-dev autoconf openssl libssl-dev libevent-dev libminiupnpc-dev g++ gcc libcurl4-openssl-dev
```

Then run the automagic build script:

```bash
./build.sh
```

Now you can run BitcoinEco as `./src/bitcoind`.

## What's broken?

Our objective is to import the current bitcoin chain state to build a genesis block.
This allows the BitcoinEco foundation to assign itself value to be used to fund improvements to the system.
It also can be exploited to generate a limited timeframe in which existing Bitcoin value may be claimed within BitcoinEco.
