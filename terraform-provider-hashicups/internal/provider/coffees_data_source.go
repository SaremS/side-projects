// Copyright (c) HashiCorp, Inc.

package provider

import (
	"context"
	"fmt"

	"github.com/hashicorp-demoapp/hashicups-client-go"
	"github.com/hashicorp/terraform-plugin-framework/datasource"
	"github.com/hashicorp/terraform-plugin-framework/datasource/schema"
	"github.com/hashicorp/terraform-plugin-framework/types"
)

var (
	_ datasource.DataSource              = &coffeesDataSource{}
	_ datasource.DataSourceWithConfigure = &coffeesDataSource{}
)

func NewCoffeesDataSource() datasource.DataSource {
	return &coffeesDataSource{}
}

type coffeesDataSource struct {
	client *hashicups.Client
}

type coffeesDataSourceModel struct {
	Coffees []coffeesModel `tfsdk:"coffees"`
}

type coffeesModel struct {
	ID          types.Int64               `tfsdk:"id"`
	Name        types.String              `tfsdk:"name"`
	Teaser      types.String              `tfsdk:"teaser"`
	Description types.String              `tfsdk:"description"`
	Price       types.Float64             `tfsdk:"price"`
	Image       types.String              `tfsdk:"image"`
	Ingredients []coffeesIngredientsModel `tfsdk:"ingredients"`
}

type coffeesIngredientsModel struct {
	ID types.Int64 `tfsdk:"id"`
}

func (d *coffeesDataSource) Metadata(_ context.Context, req datasource.MetadataRequest, resp *datasource.MetadataResponse) {
	resp.TypeName = req.ProviderTypeName + "_coffees"
}

func (d *coffeesDataSource) Schema(_ context.Context, _ datasource.SchemaRequest, resp *datasource.SchemaResponse) {
	resp.Schema = schema.Schema{
		Description: "Fetches the list of coffees.",
		Attributes: map[string]schema.Attribute{
			"coffees": schema.ListNestedAttribute{
				Description: "List of coffees",
				Computed: true,
				NestedObject: schema.NestedAttributeObject{
					Attributes: map[string]schema.Attribute{
						"id": schema.Int64Attribute{
							Description: "Numeric identifier of the coffee.",
							Computed: true,
						},
						"name": schema.StringAttribute{
							Description: "Product name of the coffee.",
							Computed: true,
						},
						"teaser": schema.StringAttribute{
							Description: "Fun tagline for the coffee",
							Computed: true,
						},
						"description": schema.StringAttribute{
							Description: "Product description of the coffee",
							Computed: true,
						},
						"price": schema.Float64Attribute{
							Description: "Suggested cost of the coffee.",
							Computed: true,
						},
						"image": schema.StringAttribute{
							Description: "URI for an image of the coffee",
							Computed: true,
						},
						"ingredients": schema.ListNestedAttribute{
							Description: "List of ingredients in the coffee",
							Computed: true,
							NestedObject: schema.NestedAttributeObject{
								Attributes: map[string]schema.Attribute{
									"id": schema.Int64Attribute{
										Description: "Numeric identifier of the coffee",
										Computed: true,
									},
								},
							},
						},
					},
				},
			},
		},
	}
}

func (d *coffeesDataSource) Read(ctx context.Context, req datasource.ReadRequest, resp *datasource.ReadResponse) {
	var state coffeesDataSourceModel

	coffees, err := d.client.GetCoffees()
	if err != nil {
		resp.Diagnostics.AddError(
			"Unable to Read HashiCups Coffees",
			err.Error(),
		)
		return
	}

	for _, coffee := range coffees {
		coffeeState := coffeesModel{
			ID:          types.Int64Value(int64(coffee.ID)),
			Name:        types.StringValue(coffee.Name),
			Teaser:      types.StringValue(coffee.Teaser),
			Description: types.StringValue(coffee.Description),
			Price:       types.Float64Value(coffee.Price),
			Image:       types.StringValue(coffee.Image),
		}

		for _, ingredient := range coffee.Ingredient {
			coffeeState.Ingredients = append(coffeeState.Ingredients, coffeesIngredientsModel{
				ID: types.Int64Value(int64(ingredient.ID)),
			})
		}

		state.Coffees = append(state.Coffees, coffeeState)
	}

	diags := resp.State.Set(ctx, &state)
	resp.Diagnostics.Append(diags...)
	if resp.Diagnostics.HasError() {
		return
	}
}

func (d *coffeesDataSource) Configure(_ context.Context, req datasource.ConfigureRequest, resp *datasource.ConfigureResponse) {
	if req.ProviderData == nil {
		return
	}

	client, ok := req.ProviderData.(*hashicups.Client)
	if !ok {
		resp.Diagnostics.AddError(
			"Unexpected Data Source Configure Type",
			fmt.Sprintf("Expected *hashicups.Client, got: %T. Please report this issue to the provider developers.", req.ProviderData),
		)

		return
	}

	d.client = client
}
