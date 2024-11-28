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
    # packages."${system}".default = with import nixpkgs {inherit system;};
    #   stdenv.mkDerivation {
    #     name = "DiscoveryTree";
    #     buildInputs = with pkgs; [cmake curl];
    #     src = self;
    #     buildPhase = ''
    #         cmake .
    #         make
    #     '';
    #     installPhase = ''
    #       cp DiscoveryTree $out
    #     '';
    #   };

    devShells."${system}".default = pkgs.mkShell {
      buildInputs = with pkgs; [cmake curl];
    };
  };
}
