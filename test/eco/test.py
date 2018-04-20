from bitcoinrpc.authproxy import AuthServiceProxy, JSONRPCException
import time, sys
# rpc_user and rpc_password are set in the bitcoin.conf file
rpc_user = 'admin'
rpc_password = 'admin'
rpc_port = ['8334','8335','8336','8337']

def generate_node_address_list(node_list, address_count):
    node_address_list = []
    for i, node in enumerate(node_list):
        node_address_list.append([])
        for x in range(address_count):
            address = node_list[i].getnewaddress()
            node_address_list[i].append(address)
    return node_address_list

def generate_blockchain(generate_block_count, node_list, node_address_list):
    block_count = 0
    while block_count < generate_block_count:
        for i, node in enumerate(node_list):
            print("node %s"%(i))
            for a, address in enumerate(node_address_list[i]):
                print("\tblock: %s"%(block_count))
                try:
                    response = node.generatetoaddress(1, address)
                    block_count += 1
                except KeyboardInterrupt:
                    exit()
                except:
                    block_count -= 1
                    response = "couldn't generate block"
                time.sleep(1)
    return block_count

def generate_stake(node_list, staking_address_list):
    for i, node in enumerate(node_list):
        print("node %s"%(i))
        for a, address in enumerate(staking_address_list[i]):
            print("\tstaking: %s"%(address))
            try:
                response = node.staketoaddress(1500, address, address)
            except KeyboardInterrupt:
                exit()
            except:
                block_count -= 1
                response = "couldn't stake"
            time.sleep(1)


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
block_count = generate_blockchain(500, node_list, node_address_list)
