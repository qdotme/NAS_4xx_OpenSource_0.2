# $Id: count.awk,v 1.1.1.1 2008/06/18 10:53:23 jason Exp $
#
# Print out the number of log records for transactions that we
# encountered.

/^\[/{
	if ($5 != 0)
		print $5
}
