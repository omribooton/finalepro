import time
import subprocess

while True:
	
	command = 'echo "Job started at $(date)" >> /home/gershwin/et_project/logfile.log && source /home/gershwin/et_project/myenv/bin/activate && echo "Virtual enviorment activated at $(date)" >> /home/gershwin/et_project/logfile.log && python3 /home/gershwin/et_project/et_penman.py >> /home/gershwin/et_project/logfile.log'
	
	subprocess.Popen(command, shell=True, executable='/bin/bash')
	

	time.sleep(3600)
