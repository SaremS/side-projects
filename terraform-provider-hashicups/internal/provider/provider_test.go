package provider

import (
	"github.com/hashicorp/terraform-plugin-framework/providerserver"
	"github.com/hashicorp/terraform-plugin-go/tfprotov6"
)

const (
	providerConfig = `
provider "hashicups" {
	username = "education"
	password = "test123"
	host 	 = "http://localhost:19090" 
}
`
)

var (
	testAccProtoV6ProviderFactories = map[string]func() (tfprotov6.ProviderServer, error){
		"hashicups": providerserver.NewProtocol6WithError(New("test")()),
	}
)
