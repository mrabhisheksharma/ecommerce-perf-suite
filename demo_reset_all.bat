@echo off
echo =========================================================================
echo RESETTING GIT DEMO ENVIRONMENT TO CLEAN BASELINE
echo =========================================================================
git merge --abort >nul 2>&1
git reset --hard 8769568
git checkout main
git status
echo.
echo Demo environment reset successfully! Ready for live demonstration.
pause

