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
    node.generate_blockchain_for_address_list()
'''
node_list = [
AuthServiceProxy("http://%s:%s@127.0.0.1:%s"%(rpc_user, rpc_password, rpc_port[0])),
AuthServiceProxy("http://%s:%s@127.0.0.1:%s"%(rpc_user, rpc_password, rpc_port[1])),
AuthServiceProxy("http://%s:%s@127.0.0.1:%s"%(rpc_user, rpc_password, rpc_port[2])),
AuthServiceProxy("http://%s:%s@127.0.0.1:%s"%(rpc_user, rpc_password, rpc_port[3])),
]

node_address_list = generate_node_address_list(node_list, 5)
for i, node in enumerate(node_address_list):
    print("node %s"%(i))
    for address in node:
        print("\t%s"%(address))

block_count = generate_blockchain(500, node_list, node_address_list)
staking_address_list = generate_node_address_list(node_list, 1)
generate_stake(node_list, staking_address_list)
block_count = generate_blockchain(1500, node_list, staking_address_list)
print(staking_address_list)
'''
