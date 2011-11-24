python setup.py sdist --format=bztar

for each in `ls dist/*.tar.bz2`
	do echo $each
	tar tvjf $each | awk '{print $6}'
done
