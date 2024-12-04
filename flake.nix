# Build, run, and package DiscoveryTree declaratively
{
  description = "DiscoveryTree Flake";
  inputs = {nixpkgs.url = "github:nixos/nixpkgs/24.05";};

  outputs = {
    self,
    nixpkgs,
  }: let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.x86_64-linux.pkgs;

    deriv = with import nixpkgs {inherit system;};
      stdenv.mkDerivation {
        name = "DiscoveryTree";
        buildInputs = with pkgs; [cmake ftxui];
        src = self;
        phases = [ "unpackPhase" "buildPhase" "installPhase"];
        buildPhase = ''
            cmake .
            make
        '';
        installPhase = ''
          mkdir -p $out/bin
          mkdir -p $out/share/discoverytree
          cp books.csv $out/bin/books.csv
          cp books.csv $out/share/discoverytree/books.csv
          cp DiscoveryTree $out/bin/DiscoveryTree
        '';
      };
  in {
    apps."${system}".default = {
        type = "app";
        program = "${deriv}/bin/DiscoveryTree";
        args = ["${deriv}/share/discoverytree/books.csv"];
      };

    packages."${system}".default = deriv;

    #Development environment toolkit
    devShells."${system}".default = pkgs.mkShell {
      buildInputs = with pkgs; [cmake curl ftxui];
    };
  };
}
