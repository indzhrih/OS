# frozen_string_literal: true

source 'https://rubygems.org'

# Bundle edge Rails instead: gem "rails", github: "rails/rails", branch: "main"
gem 'rails', '~> 7.2.2', '>= 7.2.2.2'
# Use postgresql as the database for Active Record
gem 'pg', '~> 1.1'
# Use the Puma web server [https://github.com/puma/puma]
gem 'puma', '>= 5.0'
# Build JSON APIs with ease [https://github.com/rails/jbuilder]
# gem "jbuilder"
# Use Redis adapter to run Action Cable in production
# gem "redis", ">= 4.0.1"

# Use Kredis to get higher-level data types in Redis [https://github.com/rails/kredis]
# gem "kredis"

# Use Active Model has_secure_password [https://guides.rubyonrails.org/active_model_basics.html#securepassword]
# gem "bcrypt", "~> 3.1.7"

# Windows does not include zoneinfo files, so bundle the tzinfo-data gem
gem 'tzinfo-data', platforms: %i[windows jruby]

# Reduces boot times through caching; required in config/boot.rb
gem 'bootsnap', require: false

# Shim to load environment variables from .env into ENV in development. [https://github.com/bkeepers/dotenv#rails]
gem 'dotenv-rails'

# [https://github.com/janko/image_processing]
gem 'image_processing'

# Powerful, extensible, and feature-packed frontend toolkit. [https://github.com/twbs/bootstrap-rubygem]
gem 'bootstrap', '~> 5.3.3'

# Use Dart Sass with Sprockets and the Ruby on Rails asset pipeline. [https://github.com/tablecheck/dartsass-sprockets]
gem 'dartsass-sprockets'

# [https://github.com/igorkasyanchuk/active_storage_validations]
gem 'active_storage_validations'

# A ruby wrapper for ImageMagick command line.
gem 'mini_magick'

# Use Active Storage variants [https://guides.rubyonrails.org/active_storage_overview.html#transforming-images]
# gem "image_processing", "~> 1.2"

# Use Rack CORS for handling Cross-Origin Resource Sharing (CORS), making cross-origin Ajax possible
# gem "rack-cors"

group :development, :test do
  # See https://guides.rubyonrails.org/debugging_rails_applications.html#debugging-with-the-debug-gem
  gem 'debug', platforms: %i[mri windows], require: 'debug/prelude'

  # Static analysis for security vulnerabilities [https://brakemanscanner.org/]
  gem 'brakeman', require: false

  # Omakase Ruby styling [https://github.com/rails/rubocop-rails-omakase/]
  gem 'rubocop-rails-omakase', require: false

  # Testing framework [https://github.com/rspec/rspec-rails]
  gem 'rspec-rails'

  # Factory_bot is a fixtures replacement [https://github.com/thoughtbot/factory_bot_rails]
  gem 'factory_bot_rails'

  # This gem is a port of Perl's Data::Faker library that generates fake data [https://github.com/faker-ruby/faker]
  gem 'faker'

  # Capybara how a real user would interact with your app. [https://github.com/teamcapybara/capybara]
  gem 'capybara', require: false

  # Capture a screenshot for every test failure automatically. [https://github.com/mattheworiordan/capybara-screenshot]
  gem 'capybara-screenshot'

  # [https://github.com/titusfortner/webdrivers]
  gem 'webdrivers'

  # [https://github.com/SeleniumHQ/selenium]
  gem 'selenium-webdriver'
end

group :development do
  # Use console on exceptions pages [https://github.com/rails/web-console]
  gem 'web-console'

  # rubocop [https://github.com/rubocop/rubocop]
  gem 'rubocop', require: false
  gem 'rubocop-capybara', require: false
  gem 'rubocop-performance', require: false
  gem 'rubocop-rails', require: false
  gem 'rubocop-rspec', require: false
end

group :test do
  # SimpleCov is a code coverage analysis tool for Ruby. [https://github.com/simplecov-ruby/simplecov]
  gem 'simplecov'

  # [https://github.com/rails/rails-controller-testing#rspec]
  gem 'rails-controller-testing'

  # [https://github.com/DatabaseCleaner/database_cleaner]
  gem 'database_cleaner-active_record'

  # [https://github.com/thoughtbot/shoulda-matchers]
  gem 'shoulda-matchers'
end
