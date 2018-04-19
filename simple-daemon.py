from bitcoinrpc.authproxy import AuthServiceProxy, JSONRPCException
import time, sys
# rpc_user and rpc_password are set in the bitcoin.conf file
rpc_user = 'admin'
rpc_password = 'admin'
rpc_port = '8334'
if len(sys.argv) > 1:
    rpc_port = sys.argv[1]
if len(sys.argv) > 2:
    rpc_user = sys.argv[2]
if len(sys.argv) > 3:
    rpc_password = sys.argv[3]

print("Connected to port %s"%(rpc_port))
while True:
    rpc_connection = AuthServiceProxy("http://%s:%s@127.0.0.1:%s"%(rpc_user, rpc_password, rpc_port))
    print("Getting Lotto")
    try:
        command = [ [ "getluckyaddress"] ]
        response = rpc_connection.batch_(command)[0]
        lucky_address = response['lucky_address']
        owned_address = response['owned_address']
        up_for_grabs = response['up_for_grabs']
        print(response)
        if owned_address == 'true' and up_for_grabs == 'true':
            print("Sending generate command")
            command = [['generatetoaddress', 1, lucky_address]]
            response = rpc_connection.batch_(command)[0]
            print(response)
        time.sleep(15)
    except (KeyboardInterrupt):
        break
    except:
        time.sleep(2)
