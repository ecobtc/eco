# Eco proof of stake build on Linux

## bitcoin dependencies

Follow directions in build-unix.md.

From Ubuntu 18.04 I had to install a number of packages:

```
sudo apt install build-essential libtool autotools-dev autoconf automake pkg-config libssl-dev libboost-all-dev libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler libqrencode-dev autoconf openssl libssl-dev libevent-dev libminiupnpc-dev
```
    
## libdb4.8

This cray dependency is still required by bitcoin for wallet support. Until we fork deeper we're relying on it.

## Magic build script

This builds a local copy of libdb4.8 and then builds bitcoin eco.

```
BITCOINECO_ROOT=$(pwd)
# Pick some path to install BDB to, here we create a directory within the bitcoin directory
BDB_PREFIX="${BITCOINECO_ROOT}/db4"
mkdir -p $BDB_PREFIX
# Fetch the source and verify that it is not tampered with
wget 'http://download.oracle.com/berkeley-db/db-4.8.30.NC.tar.gz'
echo '12edc0df75bf9abd7f82f821795bcee50f42cb2e5f76a6a281b85732798364ef db-4.8.30.NC.tar.gz' | sha256sum -c
# -> db-4.8.30.NC.tar.gz: OK
tar -xzvf db-4.8.30.NC.tar.gz
# Build the library and install to our prefix
cd db-4.8.30.NC/build_unix/
# Note: Do a static build so that it can be embedded into the executable, instead of having to find a .so at runtime
../dist/configure --enable-cxx --disable-shared --with-pic --prefix=$BDB_PREFIX
make install
# Configure Bitcoin Core to use our own-built instance of BDB
cd $BITCOINECO_ROOT
./autogen.sh
./configure LDFLAGS="-L${BDB_PREFIX}/lib/ -lcurl" CPPFLAGS="-I${BDB_PREFIX}/include/" CXXFLAGS="-std=c++11" && make -j 3    
```
