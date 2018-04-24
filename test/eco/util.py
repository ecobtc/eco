import time, sys
from bitcoinrpc.authproxy import AuthServiceProxy, JSONRPCException

class Node:
    def __init__(self, rpc_port, rpc_user, rpc_password):
        self.address_list = []
        self.staking_address_list = []
        self.rpc_port = rpc_port
        self.rpc_user = rpc_user
        self.rpc_password = rpc_password
        self.connect()

    def connect(self):
        self.rpc_connection = AuthServiceProxy("http://%s:%s@127.0.0.1:%s"%(self.rpc_user, self.rpc_password, self.rpc_port))
        return self.rpc_connection

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
