rm -rf ../b
mkdir ../b
touch ../b/bitcoin.conf
#echo "testnet=1" > ../b/bitcoin.conf
../../src/bitcoind -econet -rpcuser=admin -rpcpassword=admin -port=8331 -rpcport=8335 -datadir=../b -addnode=localhost:8330 -addnode=localhost:8332 -addnode=localhost:8333 -printtoconsole -reindex -discover=0
