# frozen_string_literal: true

require 'rails_helper'

RSpec.describe 'layouts/partials/_header.html.erb', type: :view do
  before { render partial: 'layouts/partials/header' }

  it "include 'tasks' label" do
    expect(rendered).to have_content('Tasks')
  end
end
