from bitcoinrpc.authproxy import AuthServiceProxy, JSONRPCException
from util import *
import time, sys
# rpc_user and rpc_password are set in the bitcoin.conf file
rpc_user = 'admin'
rpc_password = 'admin'
rpc_port = ['8334','8335','8336','8337']
node_list = []

for port in rpc_port:
    node_list.append(Node(port, rpc_user, rpc_password))

for node in node_list:
    node.generate_address_list(5)

for x in range(25):
    for node in node_list:
        node.generate_blockchain_for_address_list()

for node in node_list:
    node.generate_staking_address_list(1)
    node.generate_stake(1500)

for x in range(25):
    for node in node_list:
        node.generate_blockchain_for_address_list()
