from subprocess import Popen

args = ['./alpha_writer', 'dadda ada add']
p = Popen(args)
data = p.communicate()