{
  description = "devshell";
  nixConfig.bash-prompt = "[nix-flake] ";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs =
    { nixpkgs, ... }:
    let
      shell =
        { pkgs }:
        with pkgs;
        mkShell {
          buildInputs = [
            lldb # gdb not supported on darwin
            gcc14
          ];

          LLDB_DEBUGSERVER_PATH = "/Applications/Xcode.app/Contents/SharedFrameworks/LLDB.framework/Versions/A/Resources/debugserver";
        };
    in
    {
      devShell.aarch64-darwin = shell { pkgs = nixpkgs.legacyPackages.aarch64-darwin; };
      devShell.aarch64-linux = shell { pkgs = nixpkgs.legacyPackages.aarch64-linux; };
    };
}
