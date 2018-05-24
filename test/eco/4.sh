rm -rf ../d
mkdir ../d
touch ../d/bitcoin.conf
#echo "testnet=1" > ../d/bitcoin.conf
../../src/bitcoind -econet -rpcuser=admin -rpcpassword=admin -port=8333 -rpcport=8337 -datadir=../d -addnode=localhost:8330 -addnode=localhost:8331 -addnode=localhost:8332 -printtoconsole -reindex -discover=0
