// config.js
// Copyright (C) 2019 Regents of the Leland Stanford Junior University
//
// This module set the document specifications `host`, `owner`, `project`, `branch`, and `doc`
// as given by the URL, or if absent, as given by the current defaults
//
var package = "Docs-Browser";
var version = "0.1";
var branch = "prototype";
var logorepo = "https://raw.githubusercontent.com/slacgismo/docs-browser/" + branch + "/src/"
var doelogo = logorepo + "doe-logo.png"
var sulogo = logorepo + "stanford-logo.png";
var slaclogo = logorepo + "slac-logo.png";
var gismologo = logorepo + "gismo-logo.png";
var icon = logorepo + "icon.png";
var source = "https://github.com/slacgismo/docs-browser/tree/" + branch + "";
var website = "http://www.slacgismo.org/";
var banner = '<TABLE WIDTH="100%" CELLPADDING=0 CELLSPACING=0 BORDER=0><TR><TD VALIGN="bottom" ALIGN="left"><A HREF="' + source + '" TARGET="_blank"><TABLE><TR><TD><IMG HEIGHT=' + top_panel_height + ' SRC="' + icon + '" />&nbsp;</TD><TD ALIGN="left" VALIGN="middle"><B>' + package + '</B><BR/>Version ' + version + ' by SLAC GISMo</TD></TR></TABLE></A></TD><TD ALIGN="right" VALIGN="middle">'
	+ '<A HREF="https://www.energy.gov/science-innovation/electric-power" TARGET="_blank"><IMG HEIGHT=' + top_panel_height + ' SRC="' + doelogo + '" /></A>&nbsp;'
	+ '<A HREF="https://www.stanford.edu/" TARGET="_blank"><IMG HEIGHT=' + top_panel_height + ' SRC="' + sulogo + '" /></A>&nbsp;'
	+ '<A HREF="https://slac.stanford.edu/" TARGET="_blank"><IMG HEIGHT=' + top_panel_height + ' SRC="' + slaclogo + '" /></A>&nbsp;'
	+ '<A HREF="' + website + '" TARGET="_blank"><IMG HEIGHT=' + top_panel_height + ' SRC="' + gismologo + '" /></A>'
	+ '</TD></TR></TABLE>';
var year = (new Date()).getFullYear();
if ( year > 2019 )
{
    year = "2019-" + year;
}
var copyright = '<A HREF="https://raw.githubusercontent.com/slacgismo/docs-browser/master/LICENSE" TARGET="_blank">Copyright (C) ' + year + ', Regents of the Leland Stanford Junior University</A>';
var query = new URLSearchParams(window.location.search);

function set_default(name, value) 
{
	// console.info("set_default('" + name + "',value='" + value + "') --> cookie = '" + document.cookie + "'");
	if ( document.cookie.indexOf("BLOCKED=") >= 0 )
	{
		return;
	}
	if ( value == null )
	{
		document.cookie = name + "=;";
	}
	else
	{
		document.cookie = name + "=" + value + ";path=/; expires=36524";
	}
}

function get_default(name,deflt,asnull) 
{
	// console.info("get_default('" + name + "',value='" + deflt + "')...");
	var value = query.get(name);
	var type = 'query';
	if ( value == null )
	{
		var ca = document.cookie.split(';');
		// console.info("  cookie = " + ca);
		for ( var i = 0 ; i < ca.length ; i++ ) 
		{
			var c = ca[i].trim();
			if ( c.indexOf(name+'=') == 0 ) 
			{
				value = c.substring(c.indexOf('=')+1, c.length);
				type = 'cookie';
			}
		}
		if ( value == null )
		{
			value = deflt;
			type = 'default';
		}
	}
	if ( asnull != null && value === asnull )
	{
		value = null;
		type += ' asnull'
	}
	// console.info("  " + type + " --> " + name + " = '" + deflt + "'");
	return value;
}

var host = get_default('host',default_host,null);
var owner = get_default('owner',default_owner,null);
var project = get_default('project',default_project,null);
var branch = get_default('branch',default_branch,null);
var doc = get_default('doc',default_doc,null);
var folder = get_default('folder',"",null);
var github_authorization_token = get_default('token',null,'');
var search_keyword = get_default('search',"",null);

function save_defaults()
{
    set_default("host",host)
    set_default("owner",owner);
    set_default("project",project);
    set_default("branch",branch);
    set_default("folder",folder);
    set_default("doc",doc);
}

function run_query(query)
{
    // console.info("Running query '" + query + "'...")
    var r = new XMLHttpRequest();
    r.open('GET',query,false);
    if ( github_authorization_token != null )
    {
        // console.info("API authorization token: ",github_authorization_token);
        r.setRequestHeader("Authorization","token " + github_authorization_token);
    }
    r.send(null);
    // console.info("  status -> " + r.status);
    return r;
}
function reload_frameset()
{
    top.frames.document.location.href=top.frames.document.location.href;
}

function cookie_view()
{
	if ( document.cookie == "" )
		alert("No data stored");
	else if ( document.cookie.indexOf("BLOCKED=") >= 0 )
		alert("The cookie is blocked. Use 'Clear' to unblock it.")
	else
		alert("Current data:\n\n" + document.cookie);
}

function cookie_clear(no_confirm)
{
	if ( no_confirm || confirm("Clearing the cookie will delete all current data and unblock the cookie.\n\nIs this ok? ") )
	{
	    var cookies = document.cookie.split(";");
	    for (var i = 0; i < cookies.length; i++) {
	        var cookie = cookies[i];
	        var eqPos = cookie.indexOf("=");
	        var name = eqPos > -1 ? cookie.substr(0, eqPos) : cookie;
	        document.cookie = name + "=;expires=Thu, 01 Jan 1970 00:00:00 GMT";
	    }
	}
}

function cookie_block()
{
	if ( confirm("Blocking the cookie will delete all current data.\n\nIs this ok?") )
	{
		cookie_clear(true);
		document.cookie = "BLOCKED";
	}
}
