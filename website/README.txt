In this directory, you can create two special files to change the behavior of the website:

"under-maintenance.txt" -> Place this file in this directory to put the website into "maintenance" mode. This will show the log and a page saying that the website/hardware is currently under maintenance. You don't need to write anything in the file, its existence is enough.

"maintenance.log" -> Place this file in this directory, with each line showing a timestamp in 8601 format as well as a description of the maintenance being done. The syntax is as follows:

```
[2024-12-02T10:30:00-06:00] Description here... 
[2024-11-21T14:00:00-06:00] Description here...
```

(This is not enforced by code, but this is highly highly suggested for consistency) This will be the log that is shown to the user on the maintenance page.