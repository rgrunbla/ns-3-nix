#using Pkg
#Pkg.add("Statistics")
#Pkg.add("Measures")
#Pkg.add("StatsPlots")
#Pkg.add("Plots")
#Pkg.add("GR")
#Pkg.add("PyPlot")
#Pkg.add("Gadfly")
#Pkg.add("CSV")
#Pkg.add("DataFrames")
#Pkg.add("Glob")

using StatsPlots
gr(size=(1000,1000))
using Plots, Measures
using CSV
using DataFrames
using Glob
using Statistics

filepaths = glob("OUTPUT/Simple/*.csv")

dataframes = Dict(
    "continuous" => DataFrame(),
    "phi" => DataFrame(),
    "positions" => DataFrame(),
    "initialpositions" => DataFrame(),
    "lost" => DataFrame(),
    "summary" => DataFrame()
)

for filepath in filepaths
    splitted = split(filepath, "/")
    scenario = splitted[2]
    filename = splitted[3][1:end-4]
    parameters = split(filename, "_")
    type = parameters[1]
    df = DataFrame(CSV.File(filepath))
    for parameter in parameters[4:end]
        key, value = split(parameter, "=")
        df[!, key] .= value
    end
    dataframes[type] = vcat(dataframes[parameters[1]], df)
end

summary = dataframes["summary"]
simple_scenario = summary[summary[:,"scenarioType"] .== "0", :]
simple_scenario = simple_scenario[simple_scenario[:,"distance"] .== "100", :]
rename!(simple_scenario, ["Rx Bytes"] .=>  [:Rx_Bytes])

within = ["wifiManager", "distance", "agentType"]

# We group the experiments by wifi manager, distance, and agent type
simple_scenario[!, "colors"] = ifelse.(isequal.(simple_scenario.agentType, "Drone"), :blue, :red)

println(simple_scenario)

@df simple_scenario boxplot!(string.(:wifiManager, :distance, :agentType), :Rx_Bytes, group=(:wifiManager, color=:colors), fillalpha=0.75, linewidth=2, xrotation=40, margin=30mm)

savefig("foo.pdf")