{
  description = "DiscoveryTree Flake";
  inputs = {nixpkgs.url = "github:nixos/nixpkgs/24.05";};

  outputs = {
    self,
    nixpkgs,
  }: let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.x86_64-linux.pkgs;
  in {
    packages."${system}".default = with import nixpkgs {inherit system;};
      stdenv.mkDerivation {
        name = "DiscoveryTree";
        buildInputs = with pkgs; [cmake curl ftxui];
        src = self;
        phases = [ "unpackPhase" "buildPhase" "installPhase"];
        buildPhase = ''
            cmake .
            make
        '';
        installPhase = ''
          ls
          mkdir -p $out/bin
          mkdir -p $out/share/discoveryTree
          cp books.csv $out/share/discoverytree
          cp DiscoveryTree $out/bin/DiscoveryTree
        '';
      };

    devShells."${system}".default = pkgs.mkShell {
      buildInputs = with pkgs; [cmake curl ftxui];
    };
  };
}
