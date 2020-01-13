import os;
import json;
import datetime;

now = datetime.datetime.today();
start = datetime.datetime(now.year,now.month-1,1,0,0,0);
stop = datetime.datetime(now.year,now.month,1,0,0,0);

# get pulls closed last month
os.system('curl -s https://api.github.com/repos/slacgismo/gridlabd/pulls\\?state=closed\\&per_page=100\\&sort=updated\\&direction=desc > pulls.json');
with open("pulls.json","r") as fh:
    pulls = json.load(fh);
    fh.close()

with open("pulls.md","w") as fh:
    print('GridLAB-D Pull Requests Completed for Period',start.strftime('%m/%d/%Y'),'-',(stop-datetime.timedelta(1,0,0,0)).strftime('%m/%d/%Y'),'\n', file=fh);
    n = 1
    for pull in pulls:
        # if 'merged_at' in pull.keys() and pull['merged_at'] == None :
        #     continue
        if 'closed_at' in pull.keys() and pull['closed_at']:
            closed_at = datetime.datetime.strptime(pull['closed_at'],'%Y-%m-%dT%H:%M:%SZ');
            if start < closed_at and closed_at < stop :
                print('\n# Update #%d:'%pull['number'],pull['title'],'\n\nCompleted',closed_at.strftime('%m/%d/%Y'),'\n\n',pull['body'].replace('~~~','\n~~~'), file=fh);
                n += 1

    print('\nEnd report:',n,'pull requests closed', file=fh);
    fh.close()

if os.system('pandoc --listings -o pulls.pdf pulls.md') == 0 :
    os.system('open pulls.pdf');

# get all issues
os.system('curl -s https://api.github.com/repos/slacgismo/gridlabd/issues\\?state=open\\&per_page=1000\\&sort=created\\&direction=desc > issues.json');
with open("issues.json","r") as fh:
    issues = json.load(fh);
    fh.close()

with open("issues.md","w") as fh:
    print('GridLAB-D Open Issues as of',(stop-datetime.timedelta(1,0,0,0)).strftime('%m/%d/%Y'),'\n', file=fh);
    n = 1
    for issue in issues:
        if 'pull_request' in issue.keys() or issue['state'] != 'open':
            continue
        if 'created_at' in issue.keys():
            created_at = datetime.datetime.strptime(pull['created_at'],'%Y-%m-%dT%H:%M:%SZ');
            if created_at < stop :
                print('\n# Issue #%d:'%issue['number'],issue['title'],'\n\nOpen since',created_at.strftime('%m/%d/%Y'),'\n\n',issue['body'].replace('~~~','\n~~~'), file=fh);
                n += 1

    print('\nEnd report:',n,'issues pending resolution', file=fh);
    fh.close()

if os.system('pandoc --listings -o issues.pdf issues.md') == 0 :
    os.system('open issues.pdf');