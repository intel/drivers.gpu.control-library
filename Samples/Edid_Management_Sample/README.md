# Sample Application for EDID management APIs.

## EDID Management API
- App calls EDID management operations on all connected displays based on input args from commandline.
### Below Operations are added as Sample
- EDID Lock : Monitor or User Supplied EDID (hard coded in Sample app)
- EDID Unlock
- EDID Override with User Supplied EDID (hard coded in Sample app)
- EDID Remove
- EDID Read (Currently Active)
### Commandline Args
- Max 2 optional args can be passed
  - If no args passed all EDID management operations mentioned above will be performed on all connected displays. 
  - arg1: Operation Type. If passed only specified operation performed on all connected displays.
    - Options:
      - '-lock' : locks Monitor EDID, if called with Target ID as 2nd param, locks Supplied EDID (App hardcoded).
      - '-unlock' : unlock previous operation
      - '-override' or '-over' : Overrides EDID
      - '-remove' or '-rem' : Removed EDID
      - '-read' : Reads Current Active EDID.

  - arg2: Optional arg, target id in hex. e.g. '1040'.
