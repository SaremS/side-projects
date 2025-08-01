// Copyright (c) HashiCorp, Inc.

package provider

import (
	"testing"

	"github.com/hashicorp/terraform-plugin-testing/helper/resource"
)

func TestAccOrderResource(t * testing.T) {
	resource.Test(t, resource.TestCase{
		ProtoV6ProviderFactories: testAccProtoV6ProviderFactories,
		Steps: []resource.TestStep{
			{
				Config: providerConfig + `
resource "hashicups_order" "test" {
	items = [
		{
			coffee = {
				id = 1
			}
			quantity = 2
		},
	]
}
`,
				Check: resource.ComposeAggregateTestCheckFunc(
					resource.TestCheckResourceAttr("hashicups_order.test", "items.#", "1"),
					resource.TestCheckResourceAttr("hashicups_order.test", "items.0.quantity", "2"),
					resource.TestCheckResourceAttr("hashicups_order.test", "items.0.coffee.id", "1"),
					resource.TestCheckResourceAttr("hashicups_order.test", "items.0.coffee.description", ""),
					resource.TestCheckResourceAttr("hashicups_order.test", "items.0.coffee.image", "/hashicorp.png"),
					resource.TestCheckResourceAttr("hashicups_order.test", "items.0.coffee.name", "HCP Aeropress"),
					resource.TestCheckResourceAttr("hashicups_order.test", "items.0.coffee.price", "200"),
					resource.TestCheckResourceAttr("hashicups_order.test", "items.0.coffee.teaser", "Automation in a cup"),
					resource.TestCheckResourceAttrSet("hashicups_order.test", "id"),
					resource.TestCheckResourceAttrSet("hashicups_order.test", "last_updated"),
				), 
			},
			{
				ResourceName: "hashicups_order.test",
				ImportState: true,
				ImportStateVerify: true,
				ImportStateVerifyIgnore: []string{"last_updated"},
			},
			{
				Config: providerConfig + `
resource "hashicups_order" "test" {
	items = [
		{
			coffee = {
				id = 2
			}
			quantity = 2
		},
	]
}
`,
				Check: resource.ComposeAggregateTestCheckFunc(
					resource.TestCheckResourceAttr("hashicups_order.test", "items.0.quantity", "2"),
					resource.TestCheckResourceAttr("hashicups_order.test", "items.0.coffee.id", "2"),
					resource.TestCheckResourceAttr("hashicups_order.test", "items.0.coffee.description", ""),
					resource.TestCheckResourceAttr("hashicups_order.test", "items.0.coffee.image", "/packer.png"),
					resource.TestCheckResourceAttr("hashicups_order.test", "items.0.coffee.name", "Packer Spiced Latte"),
					resource.TestCheckResourceAttr("hashicups_order.test", "items.0.coffee.price", "350"),
					resource.TestCheckResourceAttr("hashicups_order.test", "items.0.coffee.teaser", "Packed with goodness to spice up your images"),

				),
			},
		},
	})
}
