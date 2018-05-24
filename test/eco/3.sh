rm -rf ../c
mkdir ../c
touch ../c/bitcoin.conf
#echo "testnet=1" > ../c/bitcoin.conf
../../src/bitcoind -econet -rpcuser=admin -rpcpassword=admin -port=8332 -rpcport=8336 -datadir=../c -addnode=localhost:8330 -addnode=localhost:8331 -addnode=localhost:8333 -printtoconsole -reindex -discover=0
