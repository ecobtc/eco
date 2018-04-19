rm -rf ../c
mkdir ../c
./src/bitcoind -rpcuser=admin -rpcpassword=admin -port=8332 -rpcport=8336 -datadir=../c -addnode=localhost:8330 -addnode=localhost:8331 -addnode=localhost:8333 -printtoconsole -reindex -discover=0
