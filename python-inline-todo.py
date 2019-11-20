'''
python-inline-todo.py

grabs inline TODOs and throws them into a plaintext file

by Michael Ivanitskiy (mivanits@umich.edu)
November 2019

whichever liscence lets you reproduce and modify with credit

'''


#* SETTINGS

import sys
import glob
import re


#* get command line options

# helper functions
def get_flag_idx(flags, argv = sys.argv):
	for x in flags:
		try:
			return argv.index(x)
		except:
			pass
	return None

def get_args(flags, argv = sys.argv):
	for x in flags:
		idx = get_flag_idx(flags, argv)
		if idx is not None:
			return argv[idx + 1]


# default options
OPT = {
	'searchDir' : '',
	'outfile'   : '.todo',
	'links'  	: True,
	'tag-sort'  : True,
	'file-sort' : True,
	'dir-sort'  : True,
	'format'	: 'todo+'
}

def arg_opt_replace(opt_key, flags, argv = sys.argv):
	opt_new = get_args(flags)
	if opt_new is not None:
		OPT[opt_key] = opt_new

arg_opt_replace('searchDir', ['-s', '--searchDir'])
arg_opt_replace('outfile', ['-o', '--outfile'])

# allowed formats:
# 'todo+'/'.todo'/'todo', 'markdown'/'md', 'org'/'orgmode'

# def flag_opt_replace(opt_key, flags, argv = sys.argv):




# types of comments to look for
COMMENT_TAGS = [
  "CRIT",
  "TODO",
  "FIXME",
  "FIX",
  "BUG",
  "DEBUG",
  "UGLY",
  "HACK",
  "NOTE",
  "IDEA",
  "REVIEW",
  "OPTIMIZE",
  "CONFIG",
  "!!!",
  "OLD"
]

# searches only the first `SEARCH_LEN` chars of each line
# SEARCH_LEN = 10

# defines the 
COMMENT_LINES = [
	'//',
	'/*'
	'#',
	'<!--',
	'%',
	'\\',
]

COMMENT_LINES_JOINED = ''.join(COMMENT_LINES)

# file endings to check
FILE_TYPES = [
	'c',
	'cpp',
	'h',
	'hpp',
	'py',
	'm',
	'tex',
]

# to make things faster
MAX_SEARCH_LEN = 15

REGEX_STR = '((' + '|'.join(COMMENT_LINES) + ')\\s*(' + '|'.join(COMMENT_TAGS) + '))'

#* get all valid files


# search (given) root dir, subdirs
files_search = []
for fileType in FILE_TYPES:
	glob_str = OPT['searchDir'] + '**/*.%s' % fileType
	files_search.extend( glob.glob( glob_str, recursive=True ) )

files_search = sorted([ s.replace('\\', '/') for s in files_search ])


#* search files
tag_loc = dict()
for tag in COMMENT_TAGS:
	file_dict = dict()

	for file in files_search:
		with open(file,'r') as f:
			lineNum = 1
			for line in f:
				# REVIEW: only looks for tag, not comment line. feature not a bug??
				if tag in line[:MAX_SEARCH_LEN]:
					list_temp = file_dict.get(file)
					if list_temp is None:
						list_temp = []
					list_temp.append((lineNum, line))
					file_dict[file] = list_temp
				lineNum += 1

	if len(file_dict):
		tag_loc[tag] = file_dict
	else:
		tag_loc[tag] = None

#* assemble output
outpath = OPT['searchDir'] + OPT['outfile']

with open(outpath, 'w') as fout:
	printf = lambda x : print(x, file = fout)
	
	printf('# inline TODO summary:')

	from datetime import datetime
	printf( 'Updated:\t' + str(datetime.now()))

	for tag in COMMENT_TAGS:
		dict_files = tag_loc[tag]
		if dict_files:
			printf('\n# %s:' % tag)
			for file, list_lines in dict_files.items():
				printf('\n ## `%s`:' % file)
				for line_pair in list_lines:
					line_str = line_pair[1].lstrip().lstrip(COMMENT_LINES_JOINED).lstrip().rstrip()
					printf('  [ ] %s \n  (line %d)' % (line_str,line_pair[0]) )



	


