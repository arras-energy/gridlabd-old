TUTORIALS += Using_EIA_RECS_data
TUTORIALS += Geodata

docs/Tutorials/%.md: %/tutorial.ipynb
	@echo 'Updating $<...'
	@pandoc $< -s -t gfm | sed -n '20,$$s/^\!/bash% /;20,$$p' >$@

tutorials: $(addsuffix .md,$(addprefix ../docs/Tutorials/,${TUTORIALS}))

