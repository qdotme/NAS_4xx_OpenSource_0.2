# $Id: range.awk,v 1.1.1.1 2008/06/18 10:53:23 jason Exp $
#
# Print out a range of the log

/^\[/{
	l = length($1) - 1;
	i = index($1, "]");
	file = substr($1, 2, i - 2);
	file += 0;
	start = i + 2;
	offset = substr($1, start, l - start + 1);
	i = index(offset, "]");
	offset = substr($1, start, i - 1);
	offset += 0;

	if ((file == START_FILE && offset >= START_OFFSET || file > START_FILE)\
	    && (file < END_FILE || (file == END_FILE && offset < END_OFFSET)))
		printme = 1
	else if (file == END_FILE && offset > END_OFFSET || file > END_FILE)
		exit
	else
		printme = 0
}
{
	if (printme == 1)
		print $0
}
