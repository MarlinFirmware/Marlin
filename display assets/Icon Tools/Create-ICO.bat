setlocal enabledelayedexpansion
for /f %%f in ('dir *-Icons /B /O:-D') do set f=%%f & Scripts\makeico.py %%f !f:~0,-7!