../bin/block-non-ipsec.sh
ipsec whack --xauthname 'use3' --xauthpass 'use1pass' --name road-east --initiate
ping -n -c 4 192.0.2.254
echo done
