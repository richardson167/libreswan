/testing/guestbin/swan-prep
ipsec setup start
/testing/pluto/bin/wait-until-pluto-started
ipsec auto --add west-east-auto
ipsec auto --add west-east-auto2
echo "initdone"
