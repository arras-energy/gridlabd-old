## Important Notice

This repository is a fork of the official version of GridLAB-D.  To make contributions to that version, see the PNNL-managed GitHub repository at https://github.com/gridlab-d/gridlab-d].

Authorized github users may contribute to this repository. You must be a Stanford faculty, staff, student, affiliate, or subcontractor to be a contributor.  Please contact the repository owner to be authorized as a contributor.

# SLAC Workflow
The following diagram provides an overview of the SLAC Workflow for this repository:
1. **To do**: new/reopened issues and pull requests begin here (automatic)
2. **Work in progress**: issues and pull requests move here when work begins (manually done)
3. **Review pending**: issues and pull requests move here when ready for review (automatic)
4. **Merge pending**: issues and pull requests move here when ready to merge (automatic)
5. **Done**: closed issues and pull requests move here when merge is completed (automatic)

Note: When a PR is merged docker hub will automatically update the latest images.

## Projects

SLAC has a number of active projects making contributions to the SLAC version of GridLAB-D.  Projects are encouraged to use the `master` branch. Project may use a sub-master branch named `project-master`, which can be used to consolidate PRs from multiple project-specific branches.

## Issues

New issues, bug reports, and enhancement requests should be filed using the [issue request template](issue_template.md).  Note that issues may not be picked up right away unless they are assigned to a project.

## Pull Request

A draft pull request should be opened as soon as work on the branch begins.  Please use the [pull request template](pull_request_template.md) to document your request. All discussion regarding the branch must be recorded on the PR, rather than on the issue.  

_Note_: When a merge is performed, the PR is automatically closed and the branch is deleted. The original issue(s) should also be closed when the merge is performed.  However, if the issue is not fully resolved, or if the PR has unresolved problems or questions, then new issues should be created to address each unresolved issue in the PR. 

## Documentation changes

All documentation changes arising from new features and capabilities should be made to this repository's wiki pages at https://source.gridlabd.us/wiki. Fixes or correction to GridLAB-D's online documentation should be made to https://gridlab-d.shoutwiki.org/, subject to review and approval by the PNNL team.

# Coding Conventions

The general GridLAB-D coding conventions are described at http://gridlab-d.shoutwiki.com/wiki/Programming_conventions.

# Copyrights and Licensing

All code changes are copyright of the original authors or their assignees.  All code must be licensed in accordance with the original GridLAB-D license (see https://github.com/gridlab-d/gridlab-d/blob/master/LICENSE).  Please do not contribute code which conflicts with this license. In particular, so-called "contaminating licenses" are prohibited.

