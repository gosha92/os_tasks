import os
import re
import sys
import random

def throw_var() :
	seq = [0,1]
	v = random.choice(seq)
	return v

def readdr_err() :
	print ("Error was interrupted by walk")

path = "./num"

if not os.path.exists(path):
	print ("Error: This path isnt exist")
	exit(0)

# if not os.path.exists(path) or not throw_var():
	# print ("Error: This path isnt exist")
	# exit(0)
	
if not os.path.isdir(path):
	print ("Error: This dir isnt exist")
	exit(0)

# if not os.path.isdir(path) or not throw_var():
	# print ("Error: This dir isnt exist")
	# exit(0)
	
try:
	if throw_var():
		raise NameError('walk')
	for root, _ , files in os.walk(path, onerror = readdr_err()):
		for f in files :
			nextpath = os.path.join(str(root), str(f))
			print (nextpath)		
			try:
				if throw_var():
					raise Error('open')
				nextfile = open(nextpath, 'r')
			except:
				print ('Error: Cant open ', nextpath)
			else:		
				try:
					if throw_var():
						raise Error('read')
					result = []
					for line in nextfile.readlines():
						line = ' ' + line.strip('\n')+ ' '
						line = line.replace(' ','  ')
						p = re.compile('\s(\d+)\s')
						result.extend(p.findall(line))
					result=sorted(result, key=lambda x:int(x))
					for match in result :
						print (match)
				except:
					print ('Error: Cant take lines in file', nextpath)
					
				nextfile.close()
except:
	print ('Error: Cant walk')	
		
		

