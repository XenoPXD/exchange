from progress.bar import Bar
import time
    
bar = Bar('Processing', max=200)
for i in range(200):
    bar.next()
    time.sleep(1)
bar.finish()