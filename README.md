# Steam Manifest and Lua Script Generator

This project contains a Python script that generates a Steam app manifest file (`manifest.json`) and a Lua script (`script.lua`) based on user input.

## How to Use

1.  **Run the script:**
    ```bash
    python generator.py
    ```

2.  **Follow the prompts:**
    *   The script will first ask for the details needed to generate the `manifest.json` file:
        *   App ID
        *   App Name
        *   Version
        *   Type (e.g., Application, Game)
    *   Next, it will ask for the App ID to generate the `script.lua` file.

3.  **Check the output:**
    *   The generated `manifest.json` and `script.lua` files will be saved in the same directory where the script is located.

## Example

### Manifest Generation

If you provide the following details:
*   **App ID:** `123456`
*   **App Name:** `Example Game`
*   **Version:** `1.0`
*   **Type:** `Application`

The `manifest.json` file will contain:
```json
{
    "app_id": 123456,
    "name": "Example Game",
    "version": "1.0",
    "type": "Application"
}
```

### Lua Script Generation

If you provide the App ID `123456`, the `script.lua` file will contain:
```lua
local appId = 123456
function loadApp()
    print("Loading app with ID: " .. appId)
end

-- Example of how to use the function
loadApp()
```
