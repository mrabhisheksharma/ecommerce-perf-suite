@echo off
echo =========================================================================
echo TRIGGERING LIVE GIT MERGE CONFLICT DEMONSTRATION
echo =========================================================================
echo Current Branch:
git branch --show-current
echo.
echo Attempting to merge branch 'feature/tune-think-time' into 'main'...
echo Command: git merge feature/tune-think-time
echo -------------------------------------------------------------------------
git merge feature/tune-think-time
echo -------------------------------------------------------------------------
echo.
echo NOTICE: Git stopped and reported a CONFLICT in scripts/vugen_checkout/Action.c!
echo Open scripts/vugen_checkout/Action.c to show students the conflict markers:
echo    <<<<<<< HEAD
echo    =======
echo    >>>>>>> feature/tune-think-time
echo.
echo Next steps to teach students:
echo 1. Edit the file and delete the markers (or run demo_resolve_conflict.bat)
echo 2. git add scripts/vugen_checkout/Action.c
echo 3. git commit -m "merge: resolve think time pacing between main and feature"
pause

