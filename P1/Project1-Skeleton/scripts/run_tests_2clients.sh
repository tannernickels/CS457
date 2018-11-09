#! /bin/bash

if [[ $# -lt 1 ]]
then
    echo "USAGE: $0: invalid arguments"
    echo "      - $0 -h <host_address> -u1 <username> -u2 <username> -k1 pwd1 -k2 pwd2 -t1 <test_file_name1> -t2 <test_file_name2> "
    exit -1
fi

IP=127.0.0.1
FOREIGN=false
USER_1=dcdennis
USER_2=tnickels
PWD_1=123
PWD_2=456
TEST_1=tests/test_client1.txt
TEST_2=tests/test_client2.txt
DEFAULT_USER=dcdennis

while getopts ":h:u1:u2:k1:k2:t1:t2:" option
do
    echo ${option}
    case "${option}"
    in
       h)  echo "HOSTNAME FLAG"
           IP=${OPTARG}
           FOREIGN=true
           ;;
       u1) echo "USERNAME_1 FLAG"
           USER_1=${OPTARG}
           ;;
       u2) echo "USERNAME_2 FLAG"
           USER_2=${OPTARG}
           ;;
       k1) echo "PASSWORD_1 FLAG"
           PWD_1=${OPTARG}
           ;;
       k2) echo "PASSWORD_2 FLAG"
           PWD_2=${OPTARG}
           ;;
       t1) echo "TEST_1 FLAG"
           TEST_1=${OPTARG}
           ;;
       t2) echo "TEST_2 FLAG"
           TEST_2=${OPTARG}
           ;;
       *)  echo "Invalid option"
           ;;
    esac
done
shift $((OPTIND -1))

echo $USER_1
echo $USER_2
echo "Starting Server...."
gnome-terminal -- ./scripts/run
sleep 2

echo "Starting Client 1"



if [[ ! -z $USER_1 ]]
then
    echo "Running client as $USER_1. Connecting to server at $IP"
    echo "$IP $USER_1 $ $PWD_1"
    gnome-terminal -- ./scripts/run_foreign $IP $USER_1 $TEST_1 $PWD_1
else
    echo "Running client as default user: $DEFAULT_USER. Connecting to server at $IP" 
    echo "$IP $USER_1 $TEST_1 $PWD_1"      
    gnome-terminal -- ./scripts/run_foreign $IP $USER_1 $TEST_1 $PWD_1 
fi

echo "Starting Client 2"
if [[ ! -z $USER_1 ]]
then
    echo "Running client as $USER_2. Connecting to server at $IP"
    echo "$IP $USER_2 $ $PWD_2"
    gnome-terminal -- ./scripts/run_foreign $IP $USER_2 $TEST_2 $PWD_2
else
    echo "Running client as default user: $USER_2. Connecting to server at $IP" 
    echo "$IP $USER_2 $last $PWD_2"      
    gnome-terminal -- ./scripts/run_foreign $IP $USER_2 $TEST_2 $PWD_2 
fi