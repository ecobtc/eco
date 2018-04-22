import time, sys
from bitcoinrpc.authproxy import AuthServiceProxy, JSONRPCException

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
                response = "couldn't stake"
            time.sleep(1)

class Node:
    def __init__(self, rpc_port, rpc_user, rpc_password):
        self.rpc_connection = AuthServiceProxy("http://%s:%s@127.0.0.1:%s"%(rpc_user, rpc_password, rpc_port))
        self.address_list = []
        self.staking_address_list = []

    def generate_address_list(self, address_count):
        for x in range(address_count):
            address = self.rpc_connection.getnewaddress()
            self.address_list.append(address)
        return self

    def generate_staking_address_list(self, address_count):
        for x in range(address_count):
            address = self.rpc_connection.getnewaddress()
            self.staking_address_list.append(address)
        return self

    def generate_stake(self, stake_amount):
        for address in self.staking_address_list:
            print("\tstaking: %s"%(address))
            try:
                response = self.rpc_connection.staketoaddress(1500, address, address)
            except KeyboardInterrupt:
                exit()
            except:
                response = "couldn't stake"

    def generate_blockchain_for_address(self, address):
        try:
            response = self.rpc_connection.generatetoaddress(1, address)
        except KeyboardInterrupt:
            exit()
        except:
            response = "couldn't generate block"

    def generate_blockchain_for_address_list(self):
        for address in self.address_list:
            self.generate_blockchain_for_address(address)
            time.sleep(1)
