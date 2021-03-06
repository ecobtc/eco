from bitcoinrpc.authproxy import AuthServiceProxy, JSONRPCException
from util import *
import time, sys
# rpc_user and rpc_password are set in the bitcoin.conf file
rpc_user = 'admin'
rpc_password = 'admin'
rpc_port = ['8334','8335','8336','8337']
node_list = []

def generate_blockchain(node_list, limit):
    height = 0
    while height < limit:
        for node in node_list:
            old_height = height
            try:
                node.generate_blockchain_for_address_list()
                height = int(node.getblockcount())
            except KeyboardInterrupt:
                exit()
            except:
                height = old_height
                node.connect()
            print(height)
    return node_list

def generate_stake(node):
    try:
        node.generate_staking_address_list(1)
        node.generate_stake(1500)
    except KeyboardInterrupt:
        exit()
    except:
        node.connect()
    return node

def generate_stakes(node_list):
    for node in node_list:
        generate_stake(node)
    return node_list

for port in rpc_port:
    node_list.append(Node(port, rpc_user, rpc_password))

for node in node_list:
    node.generate_address_list(5)

node_list = generate_blockchain(node_list, 500)

node_list = generate_stakes(node_list)

node_list = generate_stakes(node_list)

node_list = generate_blockchain(node_list, 1000)
