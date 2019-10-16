function debug(text)
{
    document.writeln('<CODE CLASS=DEBUG>['+text+']</CODE>');    
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
function load_markdown(root,url)
{
    var r = new XMLHttpRequest()
    r.open('GET',url,false);
    r.send(null);
    if ( r.status == 200 )
    {
        base = url.substring(0,url.lastIndexOf('/')+1);
        name = url.substring(url.lastIndexOf('/')+1);
        window.document.title = 'GridLAB-D Docs - ' + name.substring(0,name.length-3);
        var end = -1;
        var code = 0;
        var list = 0;
        for ( var start = 0 ; start >= 0 ; start = end )
        {
            end = r.responseText.indexOf('\n',start+1);
            if ( end < 0 )
            {
                break;
            }
            var line = r.responseText.substring(start,end);

            // newline -> P
            if ( line.substr(0,1) == '\n' )
            {
                if ( code == 0 )
                {
                    document.writeln('<P/>');
                }
                else
                {
                    document.writeln('<BR/>');
                }
                end = start+1;
                continue;
            }

            // ~~~ -> CODE
            if ( line.substr(0,3) == '~~~' || line.substr(0,3) == '```')
            {
                if ( code == 0 )
                {
                    document.writeln('<CODE CLASS=BLOCK>');
                    code = 1;
                }
                else
                {
                    document.writeln('</CODE>');
                    code = 0;
                }
                continue;
            }

            // block code
            if ( code == 1 )
            {
                document.writeln(line);
                continue;
            }

            // markdowns
            if ( line.match("^ *[-\*]") != null )
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
                line = '<LI>' + line.substring(1) + '</LI>';
            }
            else if ( line.match("^ *1\\.") != null )
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
                line = '<LI>' + line.substring(1) + '</LI>';
            }
            else
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
                    target = ref[0].substring(3,ref[0].length-2);
                    line = line.replace(ref[0],'<A HREF="'+root+'?url='+base+target+'.md">'+target+'</A>');
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
    }
}
