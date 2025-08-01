---
# generated by https://github.com/hashicorp/terraform-plugin-docs
page_title: "compute_tax function - hashicups"
subcategory: ""
description: |-
  Compute tax for coffee
---

# function: compute_tax

Given a price and tax rate, return the total cost including tax.

## Example Usage

```terraform
# Copyright (c) HashiCorp, Inc.

# Compute total price with tax
output "total_price" {
  value = provider::hashicups::compute_tax(5.00, 0.085)
}
```

## Signature

<!-- signature generated by tfplugindocs -->
```text
compute_tax(price number, rate number) number
```

## Arguments

<!-- arguments generated by tfplugindocs -->
1. `price` (Number) Price of coffee item.
1. `rate` (Number) Tax rate. 0.085 == 8.5%
