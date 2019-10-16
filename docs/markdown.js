function debug(text)
{
    //document.writeln('<CODE CLASS=DEBUG>['+text+']</CODE>');    
}
function error()
{
    document.writeln('<CODE CLASS=ERROR>['+text+']</CODE>');    
}
function replace_tags(line,tag,pattern,key)
{
    while ( (refs=line.match(tag+pattern+tag)) != null )
    {
        var repl = refs[0].substring(tag.length,refs[0].length-tag.length);
        //debug(refs[0]+'->'+key+':'+repl);
        line = line.replace(refs[0],'<'+key+'>'+repl+'</'+key+'>');
    }    
    return line;
}
function load_markdown(href,url)
{
    base = url.substring(0,url.lastIndexOf('/')+1);
    var r = new XMLHttpRequest()
    r.open('GET',url,false);
    r.send(null);
    if ( r.status == 200 )
    {
        var end = -1;
        var code = 0;
        var list = 0;
        for ( var start = 0 ; start >= 0 ; start = end )
        {
            end = r.responseText.indexOf('\n',start+1);
            debug(start+','+end);
            if ( end < 0 )
            {
                break;
            }
            //debug(start+','+end);
            var line = r.responseText.substring(start,end);

            // newline -> P
            if ( line.substr(0,1) == '\n' )
            {
                if ( code == 0 )
                {
                    document.writeln('<P/>');
                }
                else if ( code != -1 )
                {
                    document.writeln('<BR/>');
                }
                end = start+1;
                continue;
            }

            // ~~~ or ``` -> toggle CODE
            if ( line.substr(0,3) == '~~~' || line.substr(0,3) == '```')
            {
                if ( code == 0 )
                {
                    document.writeln('<DIV CLASS=BLOCK><CODE CLASS=BLOCK>');
                    code = -1; // suppress first NL
                }
                else
                {
                    document.writeln('</CODE></DIV>');
                    code = 0;
                }
                continue;
            }

            // block code
            if ( code != 0 )
            {
                code = 1;
                document.writeln(line);
                continue;
            }

            // --- -> HR
            if ( line.substring(0,3) == '---' )
            {
                document.writeln('<HR/>');
                continue;
            }

            // text decoration markdowns
            if ( (ref=line.match("^ *[-*]+")) != null )
            {
                if ( list == 2 )
                {
                    document.writeln('</OL>');
                    list = 0;
                }
                if ( list == 0 )
                {
                    document.writeln('<UL>');
                    list = 1;
                }
                line = '<LI>' + line.substring(ref.length) + '</LI>';
            }
            else if ( (ref=line.match("^ *1\\.")) != null )
            {
                if ( list == 1 )
                {
                    document.writeln('</UL>');
                    list = 0;
                }
                if ( list == 0 )
                {
                    document.writeln('<OL>');
                    list = 2;
                }
                line = '<LI>' + line.substring(ref.length) + '</LI>';
            }
            else if ( line.match("^[ \t]") != null )
            {
                if ( list == 1 )
                {
                    document.writeln('</UL>');
                }
                else if ( list == 2 )
                {
                    document.writeln('</OL>');
                }
                list = 0;
            }

            // #[#...] -> H
            var nh = 0;
            while ( line.substr(nh,nh+1) == '#' )
            {
                nh = nh + 1;
            }
            if ( nh > 0 )
            {
                if ( list == 1 ) document.writeln('</UL>');
                if ( list == 2 ) document.writeln('</OL>');
                list = 0;
                document.write('<H'+nh+'>'+line.substr(nh+1)+'</H'+nh+'>');
                continue;
            }

            // https://... -> HREF
            if ( (ref=line.match("https?://[-A-Za-z_@.]+/[-A-Za-z_&+?/#.]+[-A-Za-z_&+?/]")) != null )
            {
                for ( n = ref.length-1 ; n >= 0; n-- )
                {
                    repl = '<A HREF="'+ref[n]+'">'+ref[n]+'</A>';
                    line = line.replace(ref[n],repl);
                }
            }

            // [[..]]... -> HREF
            if ( (ref=line.match("\[\[[A-Za-z0-9_ ]+\]\]")) != null )
            {
                for ( n = ref.length-1 ; n >= 0; n-- )
                {
                    var m = 2;
                    if ( ref[0].charAt(m) == '[' ) m++; // not sure why this is necessary
                    target = ref[0].substring(m,ref[0].length-2);
                    line = line.replace(ref[0],'<A HREF="'+href+'?page='+base+target+'.md">'+target+'</A>');
                }
            } 

            // *...* -> I
            line = replace_tags(line,'\\*','[-A-Za-z ]+','I');
            line = replace_tags(line,'_','[-A-Za-z ]+','I');
            line = replace_tags(line,'\\*\\*','[-A-Za-z ]+','B');
            line = replace_tags(line,'__','[-A-Za-z ]+','B');
            line = replace_tags(line,'`','.+','CODE');

            // just text -> text
            document.writeln(line);
        }
        if ( list == 1 )
        {
            document.writeln('</UL>');
            list = 0;
        }
        else if ( list == 2 )
        {
            document.writeln('</OL>');
            list = 0;
        }
        debug("done")
    }
    else
    {
        error('server returned code' + r.status);
    }
}
