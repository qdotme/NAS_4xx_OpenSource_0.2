# $Id: commit.awk,v 1.1.1.1 2008/06/18 10:53:23 jason Exp $
#
# Output tid of committed transactions.

/txn_regop/ {
	print $5
}
