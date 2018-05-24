rm -rf ../f
mkdir ../f
touch ../f/bitcoin.conf
#echo "testnet=1" > ../f/bitcoin.conf
./src/bitcoind -rpcuser=admin -rpcpassword=admin -port=8339 -rpcport=8338 -datadir=../f -addnode=localhost:8330 -addnode=localhost:8331 -addnode=localhost:8332 -printtoconsole -reindex -discover=0
