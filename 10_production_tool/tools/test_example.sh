#! /bin/sh

echo "running test example for wifi"

percentage=5

while [ ${percentage} -ne 100 ]; do
    sleep 0.5s
    udp_send 127.0.0.1 2222 "wifi ${percentage}"
    percentage=$((percentage + 5))
done

udp_send 127.0.0.1 2222 "wifi ok"

echo "test_example.sh done"

exit 0
