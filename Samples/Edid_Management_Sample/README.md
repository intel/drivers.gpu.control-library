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
  - If no args passed all EDID management operations mentioned above will be performed on all connected displays. 
  - '[Operation Type]'
    - If passed only specified operation performed on all connected displays.
    - Options:
      - '-lock' : locks Monitor EDID, if called with Target ID as 2nd param, locks Supplied EDID (App hardcoded).
      - '-unlock' : unlock previous operation
      - '-over' : Overrides EDID
      - '-rem' : Removed EDID
      - '-read' : Reads Current Active EDID
      - '-help' : Display usage
  - '<-a Adapter#>' 
    - Specify adapter number [min:0, max:4]. 0 (default) - apply operation to all adapters
  - '<-t Target ID?' 
    - Specify Target ID in hex. e.g. '1040'
  - '<-e Path\To\EDID binary>' 
    - Specify EDID binary

### Usage
Edid\_Mgmt\_Sample\_app.exe [Operation] <-a Adapter#> <-t Target_ID> <-e Bbin_Filename>

### Example
Edid\_Mgmt\_Sample\_app.exe -read -a 0 -t 1040 -e IGCL_DELU2410.bin

