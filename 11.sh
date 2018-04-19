rm -rf ../a
mkdir ../a
gdb -ex=r --args ./src/bitcoind -rpcuser=admin -rpcpassword=admin -port=8330 -rpcport=8334 -datadir=../a -addnode=localhost:8331 -addnode=localhost:8332 -addnode=localhost:8333 -printtoconsole -reindex -discover=0
