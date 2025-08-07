import json

def get_app_manifest():
    """
    Prompts the user for app details and generates a Steam app manifest.
    """
    app_id = input("Enter App ID: ")
    app_name = input("Enter App Name: ")
    version = input("Enter Version: ")
    content_type = input("Enter Type (e.g., Application, Game): ")

    manifest = {
        "app_id": int(app_id),
        "name": app_name,
        "version": version,
        "type": content_type
    }

    with open("manifest.json", "w") as f:
        json.dump(manifest, f, indent=4)

    print("\\nSteam app manifest generated as manifest.json")

def generate_lua_script():
    """
    Prompts the user for an App ID and generates a Lua script.
    """
    app_id = input("Enter App ID for Lua script: ")

    lua_script = f"""\
local appId = {app_id}
function loadApp()
    print("Loading app with ID: " .. appId)
end

-- Example of how to use the function
loadApp()
"""

    with open("script.lua", "w") as f:
        f.write(lua_script)

    print("\\nLua script generated as script.lua")

if __name__ == "__main__":
    get_app_manifest()
    print("-" * 20)
    generate_lua_script()
