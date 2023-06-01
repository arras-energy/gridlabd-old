[[/Subcommand/Insights]] - Gather data on gridlabd usage

# Synopsis

	$ gridlabd insights CATEGORY [OPTIONS ...]

# Description

The `insights` utility retrieves gridlabd usage data from the AWS servers.

The following options are available.

	`-o|--output[=FILE]`	 Output result as CSV to stdout or FILE

	`-d|--debug`					 Enable debugging (raises exception instead of printing errors)

	`-h|--help|help`			 Display this help

	`-m|--month=MONTH`		 Specify the month number (1-12)

	`-o|--output=CSVFILE`	Specify the output CSV filename

	`-y|--year=YEAR`			 Specify the year number (4 digit year)

  `--signup`						 Open the ipaddr signup page to get access token

  `--token=TOKEN`				 Save the ipaddr access token (required for location category)

Currently, the supported categories are

  requests: return a list of individual requests by year, month, day, and ipaddr.

  locations: return a list of request counts by year, month, day, location

# Caveat

Collects version usage data for HiPAS GridLAB-D by year and month.  If month
is not specified, then all months of the current year are returned. If year
is not specified, then all months of all years are returned.

# See also

* [[/Subcommand/Aws]]
* [https://ipinfo.io/]
