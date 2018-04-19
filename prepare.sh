sleep 10
echo "Starting"
rm blockgenesis.sh
rm addresses
touch blockgenesis.sh
touch addresses
ADDRESSCOMMAND="$(./src/bitcoin-cli -rpcport=8334 getnewaddress)"
ADDRESS=${ADDRESSCOMMAND}
echo "./src/bitcoin-cli -rpcport=8334 generatetoaddress 1 $ADDRESS" >> blockgenesis.sh
echo "$ADDRESS">> addresses
echo "sleep 1" >> blockgenesis.sh
ADDRESSCOMMAND="$(./src/bitcoin-cli -rpcport=8334 getnewaddress)"
ADDRESS=${ADDRESSCOMMAND}
echo "$ADDRESS">> addresses
echo "./src/bitcoin-cli -rpcport=8334 generatetoaddress 1 $ADDRESS" >> blockgenesis.sh
echo "sleep 1" >> blockgenesis.sh
ADDRESSCOMMAND="$(./src/bitcoin-cli -rpcport=8335 getnewaddress)"
ADDRESS=${ADDRESSCOMMAND}
echo "./src/bitcoin-cli -rpcport=8335 generatetoaddress 1 $ADDRESS" >> blockgenesis.sh
echo "$ADDRESS">> addresses
echo "sleep 1" >> blockgenesis.sh
ADDRESSCOMMAND="$(./src/bitcoin-cli -rpcport=8335 getnewaddress)"
ADDRESS=${ADDRESSCOMMAND}
echo "./src/bitcoin-cli -rpcport=8335 generatetoaddress 1 $ADDRESS" >> blockgenesis.sh
echo "$ADDRESS">> addresses
echo "sleep 1" >> blockgenesis.sh
ADDRESSCOMMAND="$(./src/bitcoin-cli -rpcport=8336 getnewaddress)"
ADDRESS=${ADDRESSCOMMAND}
echo "./src/bitcoin-cli -rpcport=8336 generatetoaddress 1 $ADDRESS" >> blockgenesis.sh
echo "$ADDRESS">> addresses
echo "sleep 1" >> blockgenesis.sh
ADDRESSCOMMAND="$(./src/bitcoin-cli -rpcport=8336 getnewaddress)"
ADDRESS=${ADDRESSCOMMAND}
echo "./src/bitcoin-cli -rpcport=8336 generatetoaddress 1 $ADDRESS" >> blockgenesis.sh
echo "$ADDRESS">> addresses
echo "sleep 1" >> blockgenesis.sh
ADDRESSCOMMAND="$(./src/bitcoin-cli -rpcport=8337 getnewaddress)"
ADDRESS=${ADDRESSCOMMAND}
echo "./src/bitcoin-cli -rpcport=8337 generatetoaddress 1 $ADDRESS" >> blockgenesis.sh
echo "$ADDRESS">> addresses
echo "sleep 1" >> blockgenesis.sh
ADDRESSCOMMAND="$(./src/bitcoin-cli -rpcport=8337 getnewaddress)"
ADDRESS=${ADDRESSCOMMAND}
echo "./src/bitcoin-cli -rpcport=8337 generatetoaddress 1 $ADDRESS" >> blockgenesis.sh
echo "$ADDRESS">> addresses
echo "sleep 1" >> blockgenesis.sh
chmod +x blockgenesis.sh
COUNTER=0
while [  $COUNTER -lt 100 ]; do
    ./blockgenesis.sh
    let COUNTER=COUNTER+1
done
